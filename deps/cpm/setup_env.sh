#!/usr/bin/env bash


# if DRY_RUN is not defined, set it to zero
# this needs to happen before 'set', since it is an external variable

if ! [[ -v DRY_RUN ]]; then
    DRY_RUN=$DRY_RUN
fi

# setup errors
set -eu

# imports
SCRIPT_HOME=$(dirname -- "$( readlink -f -- "$0"; )";)
source $SCRIPT_HOME/vars.sh



function install_python {

    if [[ $DRY_RUN -eq 1 ]]; then
        echo "internal error: $FUNCNAME should not be called in DRY_RUN mode"
        exit 0
    fi 

    if [[ -d $PYTHON_HOME ]]; then 
        echo "cleaning previous python installation"
        rm -rf $PYTHON_HOME
    fi

    # TODO: cache python source
    mkdir $PYTHON_HOME
    echo "downloading python source code..."
    python_zip_file="$PYTHON_HOME/python.tar.gz"
    # TODO: hide wget stderr and stdout
    wget -O $python_zip_file https://www.python.org/ftp/python/$PYTHON_VERSION/Python-$PYTHON_VERSION.tgz > /dev/null

    echo "decompressing python source code"
    tar -xzf $python_zip_file -C $PYTHON_HOME
    rm -f $python_zip_file

    cd $PYTHON_HOME/Python-$PYTHON_VERSION
    echo "building python"

    # TODO:
    # para previnir que warnings de compilação sejam mostrados na tela,
    # enviar o stderr dos comandos de compilação para um arquivo, caso
    # o comando seja bem sucedido, deletar o arquivo, caso ocorra um erro,
    # avisar sobre o erro e apontar o arquivo onde os logs de erro estão
    ./configure --enable-shared --prefix $PYTHON_HOME > /dev/null
    make -j$(getconf _NPROCESSORS_ONLN) > /dev/null
    make install > /dev/null
    cd ../..
    rm -rf $PYTHON_HOME/Python-$PYTHON_VERSION

    echo -n $PYTHON_LOCAL_INSTALL_TYPE_LOCAL > $PYTHON_LOCAL_INSTALL_TYPE_PATH
    echo -n $PYTHON_VERSION > $PYTHON_VERSION_PATH
}

function install_python_shorcut {

    if [[ $DRY_RUN -eq 1 ]]; then
        echo "internal error: $FUNCNAME should not be called in DRY_RUN mode"
        exit 0
    fi

    if [[ -d $PYTHON_HOME ]]; then 
        echo "cleaning previous python installation"
        rm -rf $PYTHON_HOME
    fi

    # create bin folder
    mkdir -p $PYTHON_HOME/bin

    # create a script that calls the system python3
    echo '#!/usr/bin/env bash' > $PYTHON_BIN
    echo "$1 \"\$@\"" >> $PYTHON_BIN
    chmod +x $PYTHON_BIN


    echo -n $PYTHON_LOCAL_INSTALL_TYPE_SHORTCUT > $PYTHON_LOCAL_INSTALL_TYPE_PATH
    # no caso do shortcut, não é necessário anotar a versão do python
    # "instalado", já que caso a versão se torne inadequada, o setuper pedirá
    # que seja feita uma instalação local
}

function install_poetry {

    if [[ $DRY_RUN -eq 1 ]]; then
        echo "internal error: $FUNCNAME should not be called in DRY_RUN mode"
        exit 0
    fi

    if [[ -d $POETRY_HOME ]]; then
        echo "cleaning previous poetry installation"
        rm -rf $POETRY_HOME
    fi

    curl -sSL https://install.python-poetry.org | POETRY_HOME=$POETRY_HOME POETRY_VERSION=$POETRY_VERSION $PYTHON_BIN - > /dev/null
    $POETRY_BIN -C $SCRIPT_HOME config virtualenvs.in-project true
    $POETRY_BIN -C $SCRIPT_HOME env use $PYTHON_BIN
    $POETRY_BIN -C $SCRIPT_HOME install
    echo -n $POETRY_VERSION > $POETRY_VERSION_PATH
}

function read_answer {

    read -p "$1 ($2, $3): " answer

    if [[ "$answer" == "$2" ]]; then
        return 1
    elif [[ "$answer" == "$3" ]]; then
        return 2
    else
        echo "invalid answer, retry"
        return $(read_answer "$1" "$2" "$3")
    fi
}


# FLUXO DO PROGRAMA
# 
# a primeira parte é garantir que temos uma instalação adequada do Python
# 
# verificar se o python está instalado, se versão do python do sistema é adequada e se o sistema
# tem a biblioteca do python instalada
# -- se sim utilizar a instalação do sistema, criando atalhos na raiz do script que apontem para a
#    instalaçao do sistema
# -- se não, informar ao usuário que o python não foi encontrado, ou que a versão do python do sistema
#    não é adequada, ou que a biblioteca do python não foi encontrada.
#    perguntar se o usuário quer que o cpm faça uma instalação local automáticamente ou se o usuário
#    prefere instalar as dependências por conta própria
#    -- se o usuário preferir instalar por conta própria, encerrar o script
#    -- se o usuário preferir que seja instalado localmente, prosseguir com a instalação local



SYS_PYTHON_ERROR=0
SYS_PYTHON_ERROR_MESSAGE=0
SYS_PYTHON_CMD=0

for python in "${PYTHON_NAMES[@]}"; do

    echo -n "checking $python...: "

    SYS_PYTHON_ERROR=0
    SYS_PYTHON_ERROR_MESSAGE=0

    # check if python is installed on the system
    if ! command -v $python &> /dev/null;then
        SYS_PYTHON_ERROR=1
        SYS_PYTHON_ERROR_MESSAGE="could not find $python"
    fi

    # check if the version of system python is adequate
    if [[ $SYS_PYTHON_ERROR -eq 0 ]] && (! $version_gte $( $python $SCRIPT_HOME/scripts/get_python_version.py ) $PYTHON_VERSION); then
        SYS_PYTHON_ERROR=1
        SYS_PYTHON_ERROR_MESSAGE="the system's $python version is not adequate, CPM requires python $PYTHON_VERSION"
    fi

    # check if the system's python installation have the shared library that is needed by the pyinstaller
    if [[ $SYS_PYTHON_ERROR -eq 0 ]] && (! ldconfig -p | grep "libpython${PYTHON_REDUCED_VERSION}.so.1.0" &> /dev/null); then
        SYS_PYTHON_ERROR=1
        SYS_PYTHON_ERROR_MESSAGE="the system does not have the libpython${PYTHON_REDUCED_VERSION}.so.1.0 library"
    fi

    if [[ $SYS_PYTHON_ERROR -eq 0 ]]; then
        
        echo "SUCCESS"
        SYS_PYTHON_CMD=$python
        break
    else
        echo "FAILED, $SYS_PYTHON_ERROR_MESSAGE"
    fi
done



if [[ $SYS_PYTHON_ERROR -eq 0 ]]; then

    echo "using the system $SYS_PYTHON_CMD"

    # checar se o atalho para o python do sistema já foi criado
    # - se não, criar o atalho
    if ! [[ -d $PYTHON_HOME ]]; then
        
        echo "local shortcut for system python not found, creating..."
        
        if ! [[ $DRY_RUN -eq 1 ]]; then
            install_python_shorcut $SYS_PYTHON_CMD
        fi
    fi

    # checar se a instalação atual é local ou é apenas um atalho
    # caso seja loca, remover e fazer uma instalação atalho
    if [[ $(cat $PYTHON_LOCAL_INSTALL_TYPE_PATH) != "$PYTHON_LOCAL_INSTALL_TYPE_SHORTCUT" ]];then
    
        echo "overriding previous local installation, installing shortcut for system python..."

        if ! [[ $DRY_RUN -eq 1 ]]; then
            install_python_shorcut $SYS_PYTHON_CMD
        fi
    fi

    echo "python shortcut installation is done"

else
    
    INSTALLATION_EXISTS=$( [[ -d $PYTHON_HOME ]]; echo $? )
    INSTALLATION_IS_LOCAL=$( [[ $INSTALLATION_EXISTS -eq 0 ]] && [[ $(cat $PYTHON_LOCAL_INSTALL_TYPE_PATH) != "$PYTHON_LOCAL_INSTALL_TYPE_LOCAL" ]]; echo $? )
    
    # se a instalaçao não existir, ou existir, porém ser um atalho para o python do sistema,
    # pedir permissão ao usuário para realizar a instalação local
    if [[ $INSTALLATION_EXISTS -eq 0 ]] ||  [[ $INSTALLATION_IS_LOCAL -eq 0 ]] ; then

        echo $SYS_PYTHON_ERROR_MESSAGE
        echo "you can let CPM do a local installation of Python or you can try to resolve the dependencies yourself."
        echo "doing a local installation takes a few minutes, but not many, maybe just seconds."
        echo "it will depend on your internet connection and your processor"
        echo "" # new line
        set +e # remove restriction so we can be able to receive the result
        read_answer "do you allow CPM to do a local installation of python? 🥹 " "yes" "no" 
        input_result=$?
        set -e # put the restrictions back

        # interromper o processo caso o usuário rejeite a instalação local
        if [[ $input_result -eq 2 ]]; then

            echo "😭, ok then, after resolving the dependencies yourself, just execute this script again."
            echo ""
            exit 1
        fi
    fi


    # checar se o python local já foi instalado
    # - se não, instalar um python localmente
    if ! [[ $INSTALLATION_EXISTS -eq 0 ]] ; then
        
        echo "local python not found, installing..."
        
        if ! [[ $DRY_RUN -eq 1 ]]; then
            install_python
        fi
    fi

    # checar se a instalação atual é local ou é apenas um atalho
    # caso seja apenas um atalho, é necessário fazer uma instalação local
    if ! [[ $INSTALLATION_IS_LOCAL -eq 0 ]]; then
    
        echo "overriding previous shortcut installation, installing local python..."

        if ! [[ $DRY_RUN -eq 1 ]]; then
            install_python
        fi
    fi

    # checar se a versão do python instalada localmente é menor que a necessária
    # - se sim, atualizar o python local
    if ! $version_gte $(cat $PYTHON_VERSION_PATH) $PYTHON_VERSION; then
        
        echo "obsolete or inadequate local python version, reinstalling..."
        
        if ! [[ $DRY_RUN -eq 1 ]]; then
            install_python
        fi
    fi

    echo "python local installation is done"
fi



# checar se o poetry já está instalado
# - se não, instalar
if ! [[ -d $POETRY_HOME ]]; then

    echo "local poetry not found, installing poetry..."

    if ! [[ $DRY_RUN -eq 1 ]]; then
        install_poetry
    fi
fi

# checar se a versão do poetry instalado é menor que a da variável
# - se sim, reinstalar o poetry
if ! $SCRIPT_HOME/scripts/version_gte.py $POETRY_VERSION $(cat $POETRY_VERSION_PATH); then
    echo "obsolete or inadequate local poetry version, reinstalling..."
    
    if ! [[ $DRY_RUN -eq 1 ]]; then
        install_poetry
    fi
fi

echo "poetry local installation is done"
