# Token

cleos create account eosio eosio.token EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg

cd /eos/contracts/eosio.token

eosiocpp -o eosio.token.wasm eosio.token.cpp

cd ..
cleos set contract eosio.token eosio.token --abi eosio.token.abi -p eosio.token@active

cleos push action eosio.token create '[ "eosio", "1000000000.0000 SYS"]' -p eosio.token@active

cleos push action eosio.token issue '[ "bob", "100.0000 SYS", "memo" ]' -p eosio@active

cleos push action eosio.token transfer '[ "bob", "alice", "25.0000 SYS", "m" ]' -p bob@active

cleos get currency balance eosio.token bob SYS
cleos get currency balance eosio.token alice SYS