CURRENT_DIR=$(pwd)
WORKING_DIR="$CURRENT_DIR"/../..
TESTNET_DIR=$WORKING_DIR/testnet
TESTNET_OUTPUT_DIR=$TESTNET_DIR/testnet-local
SCRIPTS_DIR=elrond-go/scripts/testnet
SANDBOX_NAME=sandbox

cloneDependencies(){
  if [ -d "$TESTNET_DIR" ]; then
    return
  fi

  mkdir "$TESTNET_DIR"

  git clone https://github.com/ElrondNetwork/elrond-go "$TESTNET_DIR/elrond-go"
  git clone https://github.com/ElrondNetwork/elrond-deploy-go "$TESTNET_DIR/elrond-deploy-go"
  git clone https://github.com/ElrondNetwork/elrond-proxy-go "$TESTNET_DIR/elrond-proxy-go"
}

testnetRemove(){
  rm -rf "$TESTNET_OUTPUT_DIR"
}

testnetSetup(){
  mkdir "$TESTNET_OUTPUT_DIR"
  cd "$TESTNET_OUTPUT_DIR" && \
    (ln -s "$TESTNET_DIR"/elrond-go elrond-go && \
    ln -s "$TESTNET_DIR"/elrond-deploy-go elrond-deploy-go && \
    ln -s "$TESTNET_DIR"/elrond-proxy-go elrond-proxy-go)
}

testnetPrereq(){
  cd "$TESTNET_OUTPUT_DIR" && \
    bash $SCRIPTS_DIR/prerequisites.sh && \
    echo -e "export TESTNETDIR=$TESTNET_OUTPUT_DIR/$SANDBOX_NAME" > $SCRIPTS_DIR/local.sh && \
    bash $SCRIPTS_DIR/config.sh
}

testnetNew(){
  cloneDependencies
  testnetRemove
  testnetSetup
  testnetPrereq
}

testnetStart(){
  cd "$TESTNET_DIR" && \
    ./elrond-go/scripts/testnet/start.sh trace \
    sleep 3
}

testnetStop(){
  cd "$TESTNET_DIR" && \
    ./elrond-go/scripts/testnet/stop.sh
}

echoOptions(){
  echo "ERROR!!! Please choose one of the following parameters:
  - new to create a new testnet
  - start to start the testnet
  - stop to stop the testnet"
}

main(){
  if [ $# -eq 1 ]; then
    case "$1" in
      new)
        testnetNew ;;
      start)
        testnetStart ;;
      stop)
        testnetStop ;;
      *)
        echoOptions ;;
      esac
  else
    echoOptions
  fi
}

main "$@"
