# eosio-hackathon-starter

There are two ways to use the app:
* [Interact with the Jungle Testnet deployed smart contract instance](https://github.com/EOSIO/eosio-hackathon-starter#jungle-testnet)
* [Setup nodeos locally and compile then deploy the smart contract](https://github.com/EOSIO/eosio-hackathon-starter#local)

## Jungle Testnet

Follow the instructions on [EOS Jungle Testnet](https://github.com/CryptoLions/EOS-Jungle-Testnet) to install EOS, create a key pair, then register your account.
    * You do not need to run nodeos to create a key pair, so you can safely ignore those instructions if you want


### Frontend Config
1.  Update the root [.env](https://github.com/EOSIO/eosio-hackathon-starter/blob/master/frontend/.env) with your account name, generated private key, chain ID, and update `REACT_APP_EOS_ENV=test` like below
    ```bash
    REACT_APP_EOS_ENV=test
    REACT_APP_EOS_ACCOUNT=
    REACT_APP_EOS_PRIVATE_KEY=
    REACT_APP_EOS_CHAIN_ID=

    REACT_APP_EOS_LOCAL_CONTRACT_ACCOUNT=blog
    REACT_APP_EOS_TEST_CONTRACT_ACCOUNT=testblogeos

    REACT_APP_EOS_LOCAL_HTTP_URL=http://localhost:8888
    REACT_APP_EOS_TEST_HTTP_URL=http://jungle.cryptolions.io:38888
    ```
    * You can get the chainId via the command:
    ```bash
    cleos get info
    ```
2.  Start the react app
    ```bash
    cd frontend
    npm start
    ```
3. You should be able to view a view sample blog posts, but your account does not have the neccessary authorization to create, edit, or delete posts. However, there is not required auth for the like action, so you can like posts, which will use the private key you set in the .env to sign the transaction to like the post


## Local

Instructions and implementation written and tested for EOSIO v1.0.7

### Prerequisites

You have completed the steps in [Building EOSIO](https://developers.eos.io/eosio-nodeos/docs/getting-the-code) from the EOSIO Developer Portal with keosd & nodeos running.

Make sure to checkout the tag v1.0.7
```bash
git checkout tags/v1.0.7
git submodule update --init --recursive
```

Before creating an account to deploy the contract you have to create a default wallet for the eosio account and import the eosio private key.

#### Default wallet & eosio account

1.  Create the default wallet
    * Be sure to save this password somewhere safe. This password is used to unlock (decrypt) your wallet file.
    ```bash
    # cleos wallet create
    cleos wallet create 
    ```
2.  Import the eosio private key into the default wallet
    Find your key pair in config.ini, which can be found at:
    * Mac OS: ~/Library/Application Support/eosio/nodeos/config
    * Linux: ~/.local/share/eosio/nodeos/config
    ```bash
    # cleos wallet import <Private Key>
    cleos wallet import 
    ```

### Getting Started

* cleos, keosd, and nodeos executables can be found in the `eos/build/programs` folder after successfully building the project
* eosiocpp executable can be found in the `eos/build/tools` folder 
* They should already be available in your path

When running cleos if you get an error that it is unable to connect to keosd, execute the following:
1.  Modify the wallet config.ini
    * ~/eosio-wallet/config.ini
2.  Change `http-server-address = 127.0.0.1:8889`
3.  Run keosd
    ```bash
    keosd
    ```
4.  Specify the wallet url when running cleos
    ```bash
    cleos --wallet-url="http://localhost:8889/"
    ```

### Contract Deployment

1.  Create a wallet
    * Save the password. You will need it to unlock your wallet
    ```bash
    # cleos wallet create -n <Wallet Name>
    cleos wallet create -n blog.platform
    ```
2.  Create keys - owner and active keys
    * Save the private keys. You will need them to add the keys to the wallet
    ```bash
    cleos create key
    cleos create key
    ```
3.  Import keys into wallet
    ```bash
    # cleos wallet import <Private Key> -n <Wallet Name>
    cleos wallet import <Owner Private Key> -n blog.platform
    cleos wallet import <Active Private Key> -n blog.platform
    ```
4.  Create accounts for blog smart contract
    * Remember that accounts have no relation with keys in wallets, aside from when you sign transactions to make calls to your contract with keys
    ```bash
    # cleos create account eosio <Account> <Owner Public Key> <Active Public Key>
    cleos create account eosio blog <Owner Public Key> <Active Public Key>
    ```
5.  Compile contract to webassembly with eosiocpp
    * If you have trouble compiling, run `sudo make install` in the `eos/build` directory
    ```bash
    # eosiocpp -o <Target> <Smart Contract File>
    eosiocpp -o contract/blog.wast contract/blog.cpp
    ```
6.  Generate ABI file
    ```bash
    # eosiocpp -g <Target> <Smart Contract File>
    eosiocpp -g contract/blog.abi contract/blog.cpp
    ```
7.  Deploy contract
    ```bash
    # cleos set contract <Account> <Path to contract folder> <Path to .wast file> <Path to .abi file>
    cleos set contract blog ./contract ./contract/blog.wast ./contract/blog.abi
    ```
8.  Create a blog post
    ```bash
    # cleos push action <Account> <Action name> '<Data>' -p <Account>@active
    cleos push action blog createpost '["blog", "Sample Blog Title", "Sample blog content blah blah", "misc"]' -p blog@active
    ```
9.  Check that the blog post was added to the table
    ```bash
    # cleos get table <Contract> <Scope> <Table>
    cleos get table blog blog post
    ```

#### Frontend Config

1.  Update the root [.env](https://github.com/EOSIO/eosio-hackathon-starter/blob/master/frontend/.env) with your generated private key, chain ID, and update `REACT_APP_EOS_ENV=local` & `REACT_APP_EOS_ACCOUNT=blog` like below 
    ```bash
    REACT_APP_EOS_ENV=local
    REACT_APP_EOS_ACCOUNT=blog
    REACT_APP_EOS_PRIVATE_KEY=
    REACT_APP_EOS_CHAIN_ID=

    REACT_APP_EOS_LOCAL_CONTRACT_ACCOUNT=blog
    REACT_APP_EOS_TEST_CONTRACT_ACCOUNT=testblogeos

    REACT_APP_EOS_LOCAL_HTTP_URL=http://localhost:8888
    REACT_APP_EOS_TEST_HTTP_URL=http://jungle.cryptolions.io:38888
    ```
    * You can get the chainId via the command:
    ```bash
    cleos get info
    ```
2.  Start the react app
    * If you get a CORS error when running the app, modify the nodeos config.ini
        * Mac OS: ~/Library/Application Support/eosio/nodeos/config
        * Linux: ~/.local/share/eosio/nodeos/config
    * Set `access-control-allow-origin = *`
    ```bash
    cd frontend
    npm start
    ```