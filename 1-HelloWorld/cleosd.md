# 常用 cleos 命令

cleos push action hello hi '{"name":"bob"}'

cleos push action eosio.token transfer '{"from":"eosjustaward","to":"justtest2222","quantity":"1000.0000 EOS","memo":""}' -p eosaccount

cleos get account bob

cleos set contract hello ~/projects/eos/lesson/examples/build/hello

