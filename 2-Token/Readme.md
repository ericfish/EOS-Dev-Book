# Token 部署到 eosio.token 账户

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

# Token 部署到 SelfToken 账户

cleos create account eosio selftoken EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg
cleos create account eosio selftoken2 EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg
cleos create account eosio selftoken3 EOS5TGs5dGK4BMC6Qwc2mbWzp1wn1pjrY3D1JFeybb22vRJ1qeLGg

cd /eos/contracts/eosio.token

eosiocpp -o eosio.token.wasm eosio.token.cpp

cd ..
cleos set contract selftoken eosio.token --abi eosio.token.abi -p selftoken@active

cleos push action selftoken create '[ "selftoken2", "1000000000.0000 SYS"]' -p selftoken@active

cleos push action selftoken issue '[ "selftoken2", "100.0000 SYS", "memo" ]' -p selftoken2@active

cleos get currency balance selftoken selftoken2 SYS

cleos push action selftoken transfer '[ "selftoken2", "selftoken3", "35.0000 SYS", "m" ]' -p selftoken2@active

cleos get currency balance selftoken selftoken3 SYS

