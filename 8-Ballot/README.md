# EOS 投票工具说明

## 合约开发步骤

1 基础框架

2 数据结构

3 构造函数

4 函数

5 ABI

## 合约部署和测试

1 创建和配置 Scatter 插件中的合约测试账号

2 https://dev4eos.com/#/ 编译并部署合约

选择 ballot.cpp 为 Main

检查部署账户有足够的内存

如果部署失败，试下浏览器挂梯子

3 https://kylin.bloks.io 对合约的 Action 进行测试并查看 Table 数据

## 前端开发步骤

1 设计界面原型

2 完成基础登录登出模块配置

注：测试网与主网交叉登录时，需要清理Scatter账户缓存，否则会导致账号登录失败

3 绑定 Table 数据

注：multi-index 索引可以对索引的列做条件过滤

4 业务逻辑关联 Action