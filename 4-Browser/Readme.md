# Browser

## get_info
获取节点相关的最新信息
http://api.eosbeijing.one/v1/chain/get_info

## get_block
获取一个块的信息
http://api.eosbeijing.one/v1/chain/get_block

## get_account
获取账户的信息
http://api.eosbeijing.one/v1/chain/get_account

## get_currency_balance
获取账户的Token余额
http://api.eosbeijing.one/v1/chain/get_currency_balance

## get_account history
获取账户的转账记录

注：History 插件太占资源，一般不开放
http://api.eosbeijing.one/v1/history/get_account

所以使用 EOS Park 的接口（or 需要使用 mongodb 同步区块数据）
https://developer.eospark.com/api-doc/account/#get-account-transactions

## get_transaction history
获取转账记录详情

注：History 插件太占资源，一般不开放
http://api.eosbeijing.one/v1/history/get_transaction

所以使用 EOS Park 的接口（or 需要使用 mongodb 同步区块数据）
https://developer.eospark.com/api-doc/transaction/#get-transaction

## EOS Park API

https://developer.eospark.com/api-doc/

## 测试地址：

https://ericfish.github.io/eosbrowser/
