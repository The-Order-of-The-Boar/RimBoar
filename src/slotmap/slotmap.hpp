#pragma once

// builtin
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <optional>
#include <variant>
#include <vector>

// local
#include "../utils/logging/assert.hpp"



template <typename T>
class SlotMap {

private:

    // it is theoretically and practically impossible for an item to exist with the maximum
    // position, since the smallest possible slot has m_size greater than 1 byte
    static inline size_t NONE = SIZE_MAX;
    using Version = uint64_t;

    enum class SlotType {
        Empty,
        Occupied
    };

    class Slot {

    private:

        Version m_version;
        SlotType m_type;
        size_t m_idx_or_next_free;

    private:

        Slot(Version version, SlotType type, size_t idx_or_next_free):
            m_version(version), m_type(type), m_idx_or_next_free(idx_or_next_free) {}

    public:

        Slot(Slot&&) noexcept = default;
        Slot& operator=(Slot&&) noexcept = default;
        Slot(Slot const&) = delete;
        Slot& operator=(Slot const&) = delete;

        static Slot new_empty(size_t next_free) {

            return Slot{0, SlotType::Empty, next_free};
        }

        void to_occupied(size_t idx) {

            rb_runtime_assert(m_type == SlotType::Empty);
            m_type = SlotType::Occupied;
            m_idx_or_next_free = idx;
        }

        void to_empty(size_t next_free) {

            rb_assert(m_type == SlotType::Occupied);
            m_type = SlotType::Empty;
            m_idx_or_next_free = next_free;
            m_version += 1;
        }

        size_t new_idx(size_t idx) {

            rb_assert(m_type == SlotType::Occupied);
            auto output = m_idx_or_next_free;
            m_idx_or_next_free = idx;
            return output;
        }

        [[nodiscard]]
        bool is_empty() const {

            return m_type == SlotType::Empty;
        }

        [[nodiscard]]
        bool is_occupied() const {

            return m_type == SlotType::Occupied;
        }

        [[nodiscard]]
        size_t get_next_free() const {

            rb_assert(this->is_empty());
            return this->m_idx_or_next_free;
        }

        [[nodiscard]]
        size_t get_idx() const {

            rb_assert(this->is_occupied());
            return this->m_idx_or_next_free;
        }

        [[nodiscard]]
        Version get_version() const {

            return m_version;
        }
    };
    static_assert(sizeof(Slot) > 1);

    struct DataSlot {

        size_t m_slot_idx;
        T m_value;
    };

    class Iterator {

        // todo: impl
    };


public:

    class Key {

        friend SlotMap;

        Version m_version;
        size_t m_idx;

        Key(Version version, size_t idx): m_version(version), m_idx(idx) {}

    public:
        
        [[nodiscard]]
        static Key __build_key(size_t idx, Version version) {

            return Key{version, idx};
        }
        
        [[nodiscard]]
        size_t get_idx() const {
            
            return m_idx;
        }
        
        [[nodiscard]]
        Version get_version() {
            
            return m_version;
        }
    };

private:

    size_t m_next_free_head;
    std::vector<Slot> m_slots;
    std::vector<DataSlot> m_data;
    size_t m_size; // número de m_slots ocupados e de items em 'm_data'

public:

    // public funcs

    SlotMap(): m_next_free_head(NONE), m_slots(), m_data(), m_size(0) {}

    [[nodiscard]]
    bool contains(Key key) const {

        if (key.m_idx >= m_slots.size())
            return false;

        auto& slot = m_slots[key.m_idx];

        if (slot.is_empty())
            return false;

        if (slot.get_version() != key.m_version)
            return false;

        return true;
    }

    [[nodiscard]]
    T& get(Key key) {

        rb_runtime_assert(this->contains(key));
        return m_data[m_slots[key.m_idx].get_idx()].m_value;
    }

    Key push(T&& new_item) {

        // aloca um novo slot vazio caso não haja nenhum disponível
        if (m_next_free_head == NONE)
            this->push_empty_slot();

        rb_assert(m_next_free_head != NONE);
        rb_assert(m_next_free_head < m_slots.size());

        // reserva o primeiro slot vazio da fila para ser o slot do novo item
        size_t slot_idx = m_next_free_head;
        Slot& slot = m_slots[slot_idx];
        rb_assert(slot.is_empty());

        // atribuí o segundo item na lista de slots livres para a primeira posição
        m_next_free_head = slot.get_next_free();

        // garante que o próximo item na lista de slots livres é válido
        if (m_next_free_head != NONE)
            rb_assert(m_slots[m_next_free_head].is_empty());

        // insere o novo item no vetor contínuo, apontando para o slot reservado
        m_data.push_back(DataSlot{slot_idx, new_item});

        // muda o estado do slot reservado para ocupado, fazendo ele apontar para a posição
        // do valor no vetor contínuo
        slot.to_occupied(m_data.size() - 1);

        rb_assert(slot.get_idx() == m_data.size() - 1);
        rb_assert(m_data.back().m_slot_idx == slot_idx);

        m_size += 1;

        auto key = Key{slot.get_version(), slot_idx};
        rb_assert(this->contains(key));
        return key;
    }

    T remove(Key key) {

        rb_runtime_assert(this->contains(key));

        auto slot_idx = key.m_idx;
        auto& slot = m_slots[slot_idx];
        auto data_idx = slot.get_idx();

        // torna o slot vazio e coloca ele no início da lista de slots vazios
        // colocando o primeiro anterior na segunda posição;
        slot.to_empty(m_next_free_head);
        m_next_free_head = slot_idx;

        T item = this->swap_remove_data(data_idx);
        m_size -= 1;

        return item;
    }

    size_t size() const {
        
        return m_size;
    }

    [[nodiscard]]
    size_t capacity() {
        
        return m_slots.size();
    }


private:

    size_t push_empty_slot() {

        m_slots.push_back(Slot::new_empty(NONE));
        m_next_free_head = m_slots.size() - 1;
    }

    T swap_remove_data(size_t idx) {

        rb_assert(idx < m_data.size());

        if (idx == m_data.size() - 1) {

            T output = std::move(m_data[m_data.size() - 1].m_value);
            m_data.pop_back();
            return output;
        }

        std::swap(m_data[idx], m_data[m_data.size() - 1]);
        T output = std::move(m_data.begin()->m_value);
        m_data.pop_back();

        // atualiza o index do vetor contínuo do slot que entrou na posição do slot removido
        auto old_idx = m_slots[m_data[idx].m_slot_idx].new_idx(idx);
        rb_assert(old_idx == m_data.size() - 0);

        return output;
    }
};