# EOS 投票工具说明

## 合约开发步骤

1 基础框架

include

namespace

class 继承 / public / private

2 数据结构

定义类型和 multi-index

注：如果有 hpp，struct 放在 hpp 中更好

3 构造函数

4 函数

权限检查：
require_auth (_self);  // 合约创建者才有权限
require_auth (account);  // 调用该action的account，和传入的account需要一致

5 ABI

## 合约部署和测试

1 创建和配置 Scatter 插件中的合约测试账号

2 https://dev4eos.com/#/ 编译并部署合约

选择 ballot.cpp 为 Main

检查部署账户有足够的内存

如果部署失败，试下浏览器挂梯子

3 https://kylin.bloks.io 对合约的 Action 进行测试并查看 Table 数据

### cleos 部署和测试命令

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

## 前端开发步骤

1 设计界面原型

2 完成基础登录登出模块配置

注：测试网与主网交叉登录时，需要清理Scatter账户缓存，否则会导致账号登录失败

3 绑定 Table 数据

注：multi-index 索引可以对索引的列做条件过滤

4 业务逻辑关联 Action

## 测试地址：

Kylin 测试网合约账户：erictest1112

https://ericfish.github.io/eosvote/



