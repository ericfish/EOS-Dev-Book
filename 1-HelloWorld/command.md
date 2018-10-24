# DOCKER

docker pull eosio/eos-dev

docker network create eosdev

docker run --name nodeos02 -d -p 8888:8888 --network eosdev \
-v /tmp/eosio/work:/work -v /tmp/eosio/data:/mnt/dev/data \
-v /tmp/eosio/config:/mnt/dev/config eosio/eos-dev  \
/bin/bash -c "nodeos -e -p eosio --plugin eosio::producer_plugin \
--plugin eosio::history_plugin --plugin eosio::chain_api_plugin \
--plugin eosio::history_api_plugin \
--plugin eosio::http_plugin -d /mnt/dev/data \
--config-dir /mnt/dev/config \
--http-server-address=0.0.0.0:8888 \
--access-control-allow-origin=* --contracts-console --http-validate-host=false"

docker run -d --name keosd --network=eosdev \
-i eosio/eos-dev /bin/bash -c "keosd --http-server-address=0.0.0.0:9876"


## 检查钱包 in keosd 容器
docker exec -it keosd bash
cleos --wallet-url http://127.0.0.1:9876 wallet list keys
exit


## 为Cleos（交互终端）创建快捷方式

docker network inspect eosdev

alias cleos='docker exec -it nodeos /opt/eosio/bin/cleos --url http://127.0.0.1:8888 --wallet-url http://172.18.0.3:9876'

cleos version client


## 启动和停止容器

docker ps –l

docker stop nodeos

docker restart nodeos

## 验证

docker logs --tail 10 nodeos

curl http://localhost:8888/v1/chain/get_info


# 本地编译

cd /Users/ericfish/Code/038-eos/

git clone https://github.com/EOSIO/eos --recursive

cd eos
./eosio_build.sh

./eosio_install.sh

## 本地启动

cd build/programs/nodeos
./nodeos -e -p eosio --plugin eosio::chain_api_plugin

## 版本

cd eos

git tag

git checkout -b v1.4.0 v1.4.0



# 钱包操作

cleos wallet create --to-console

- PW5JfS1KQNwrkmdTTNgEBFAeNGUTHGiatx5rstxR1rMXRLfUq3x7A

cleos wallet open

cleos wallet list

cleos wallet unlock

cleos wallet list

cleos wallet create_key

- EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg
- 5JzSEHuBzkjnCm3dfFPrt9w9N4n35CSTtvosBGJB6i8FU5bYqwY

cleos wallet private_keys

cleos wallet import
# 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3 - eosio

cleos create account eosio bob EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg
cleos create account eosio alice EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg


# 编译部署合约

cd /work/hello

cleos create account eosio hello EOS67oPbbm…

eosiocpp -o hello.wasm hello.cpp

eosiocpp -g hello.abi hello.cpp

cd ..

cleos set contract hello ./hello

cleos push action hello hi '["bob"]' -p bob@active



# 开发工具 - 安装 CDT

git clone --recursive https://github.com/eosio/eosio.cdt

cd eosio.cdt 

./build.sh SYS

sudo ./install.sh

which eosio-cpp


# ballot合约

eosiocpp -g ballot.abi ballot.cpp

eosiocpp -o ballot.wast ballot.cpp

cleos create account eosio ballot EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg

cd ..
cleos set contract ballot ballot

cleos get table ballot ballot vote
cleos get table ballot ballot proposal
cleos get table ballot ballot ppslvoter

cleos push action ballot addvote '["bob","中午吃什么？"]' -p bob@active

cleos push action ballot addproposal '["bob",0,"时差七小时"]' -p bob@active
cleos push action ballot addproposal '["bob",0,"汉堡王"]' -p bob@active

cleos push action ballot startvote '["bob",0]' -p bob@active

cleos push action ballot voteproposal '["bob",0]' -p bob@active

cleos push action ballot delvoteppsl '["hello",0]' -p hello@active

cleos push action ballot winproposal '["bob",0]' -p bob@active

