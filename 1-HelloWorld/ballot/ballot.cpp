#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/contract.hpp>
#include <string>

using namespace eosio;
using std::string;

class ballot : public eosio::contract
{
public:
  const uint32_t FIVE_MINUTES = 5 * 60;

  ballot(account_name self)
      : eosio::contract(self),
        proposals(self, self),
        votes(self, self),
        ppslvoters(self, self) {}

  // 发起一次投票
  // @abi action
  void addvote(account_name account, string vote_name)
  {
    require_auth(account);
    // 如果同名的投票存在就退出
    for (auto &item : votes)
    {
      if (item.vote_name == vote_name)
      {
        print("vote ", vote_name, " exist!");
        return;
      }
    }
    // 存储新的投票记录
    votes.emplace(_self, [&](auto &vote) {
      vote.key = votes.available_primary_key();
      vote.vote_name = vote_name;
      vote.created = eosio::time_point_sec(now());
      vote.deadline = eosio::time_point_sec(0);
      vote.owner = account;
      vote.voting = false;
    });
  }

  // 开始投票
  // @abi action
  void startvote(account_name account, uint64_t vote_key)
  {
    require_auth(account);
    auto vtit = votes.find(vote_key);
    eosio_assert(vtit != votes.end(), "vote not exist");
    eosio_assert(vtit->owner == account, "only owner can start the vote");
    eosio_assert(!vtit->voting, "vote is already voting");
    // 更新投票截止时间
    votes.modify(vtit, 0, [&](auto &vote) {
      vote.deadline = time_point_sec(time_point_sec(now()) + FIVE_MINUTES);
      vote.voting = true;
    });
  }

  // 添加投票提案
  // @abi action
  void addproposal(account_name account, uint64_t vote_key, string proposal_name)
  {
    require_auth(account);
    auto it = votes.find(vote_key);
    eosio_assert(it != votes.end(), "vote does not exist");
    eosio_assert(!it->voting, "only can add proposal before vote starts");
    eosio_assert(it->owner == account, "only vote owner can add proposal");
    // 如果提案内容出现过就不新增
    auto vote_index = proposals.get_index<N(vote_key)>();
    auto itr = vote_index.find(vote_key);
    for (; itr != vote_index.end() && itr->vote_key == vote_key; ++itr)
    {
      if (itr->name == proposal_name)
      {
        print("proposal ", proposal_name, " exists!");
        return;
      }
    }
    // 新增提案
    proposals.emplace(_self, [&](auto &proposal) {
      proposal.key = proposals.available_primary_key();
      proposal.vote_key = vote_key;
      proposal.name = proposal_name;
      proposal.vote_count = 0;
    });
  }

  // 投票给某个提议
  // @abi action
  void voteproposal(account_name voter, uint64_t proposal_key)
  {
    require_auth(voter);
    // 检查提议是否存在
    auto pit = proposals.find(proposal_key);
    eosio_assert(pit != proposals.end(), "invalid proposal");
    // 检查投票是否处于正确的状态
    auto vtit = votes.find(pit->vote_key);
    eosio_assert(vtit != votes.end(), "invalid vote");
    eosio_assert(vtit->voting, "vote is not started");
    eosio_assert(time_point_sec(now()) < vtit->deadline, "vote is ended");
    // 用户只能给自己的投票添加提议
    auto owner_index = ppslvoters.get_index<N(owner)>();
    auto itr = owner_index.find(voter);
    for (; itr != owner_index.end() && itr->owner == voter; ++itr)
    {
      if (itr->proposal_key == proposal_key)
      {
        print("proposal voted!");
        return;
      }
    }

    ppslvoters.emplace(_self, [&](auto &ppslvoter) {
      ppslvoter.key = ppslvoters.available_primary_key();
      ppslvoter.owner = voter;
      ppslvoter.proposal_key = proposal_key;
    });

    proposals.modify(pit, _self, [&](auto &a) {
      a.vote_count += 1;
    });
  }

  // 删除给某个提议的投票
  // @abi action
  void delvoteppsl(account_name voter, uint64_t proposal_key)
  {
    require_auth(voter);

    auto pit = proposals.find(proposal_key);
    eosio_assert(pit != proposals.end(), "invalid proposal");

    auto vtit = votes.find(pit->vote_key);
    eosio_assert(vtit != votes.end(), "invalid vote");
    eosio_assert(vtit->voting, "vote is not started");
    eosio_assert(time_point_sec(now()) < vtit->deadline, "vote is ended");

    // 查找并删除投票记录
    auto owner_index = ppslvoters.get_index<N(owner)>();
    auto itr = owner_index.find(voter);
    for (; itr != owner_index.end() && itr->owner == voter; ++itr)
    {
      if (itr->proposal_key == proposal_key)
      {
        proposals.modify(pit, _self, [&](auto &a) {
          a.vote_count -= 1;
        });
        owner_index.erase(itr);
        return;
      }
    }
  }

  // 返回投票数最多的提议
  // @abi action
  void winproposal(account_name account, uint64_t vote_key)
  {
    require_auth(account);
    auto vtit = votes.find(vote_key);
    eosio_assert(vtit != votes.end(), "vote not exist");
    eosio_assert(time_point_sec(now()) > vtit->deadline, "voting, no winner");
    eosio_assert(vtit->owner == account, "only owner can set win proposal");

    std::vector<uint64_t> winners;
    // 查找最大值
    uint64_t max = 0;
    auto vote_index = proposals.get_index<N(vote_key)>();
    auto itr = vote_index.find(vote_key);
    for (; itr != vote_index.end() && itr->vote_key == vote_key; ++itr)
    {
      print("proposal ", itr->key, " = ", itr->name, " \n");
      if (itr->vote_count > max)
      {
        max = itr->vote_count;
      }
    }
    print("max vote ", max, "\n");

    // 打印得票最多的提议
    if (max > 0)
    {
      auto vote_index = proposals.get_index<N(vote_key)>();
      auto itr = vote_index.find(vote_key);
      for (; itr != vote_index.end() && itr->vote_key == vote_key; ++itr)
      {
        if (itr->vote_count == max)
        {
          winners.push_back(itr->key);
          print("win proposal is: ", itr->name, " vote count", itr->vote_count, "\n");
        }
      }
      votes.modify(vtit, _self, [&](auto &vote) {
        vote.winner = winners;
      });
    }
    else
    {
      print("no vote", "\n");
    }
  }

private:
  // 定义一次投票活动
  //@abi table vote i64
  struct vote
  {
    uint64_t key;
    string vote_name;             // 活动名
    account_name owner;           // 发起人
    time_point_sec created;       // 创建时间
    time_point_sec deadline;      // 投票截止时间
    bool voting;                  // 可以投票
    std::vector<uint64_t> winner; // 获胜提案id

    uint64_t
    primary_key() const
    {
      return key;
    }

    EOSLIB_SERIALIZE(vote, (key)(vote_name)(owner)(created)(deadline)(voting)(winner));
  };
  typedef eosio::multi_index<N(vote), vote> vote_index;

  // 对一个提案投票的记录
  //@abi table ppslvoter i64
  struct ppslvoter
  {
    uint64_t key;
    uint64_t proposal_key; // 提案ID
    account_name owner;    // 投票人

    uint64_t primary_key() const { return key; }
    uint64_t by_owner() const { return owner; }

    EOSLIB_SERIALIZE(ppslvoter, (key)(proposal_key)(owner));
  };
  typedef eosio::multi_index<N(ppslvoter), ppslvoter,
                             indexed_by<N(owner), const_mem_fun<ppslvoter, uint64_t, &ppslvoter::by_owner>>>
      ppslvoter_index;

  // 提案的数据结构
  //@abi table proposal i64
  struct proposal
  {
    uint64_t key;
    uint64_t vote_key;   // 投票ID
    string name;         // 提案的名称
    uint64_t vote_count; // 提议接受的投票数

    uint64_t primary_key() const { return key; }
    uint64_t by_vote_key() const { return vote_key; }

    EOSLIB_SERIALIZE(proposal, (key)(vote_key)(name)(vote_count));
  };
  typedef eosio::multi_index<N(proposal), proposal,
                             indexed_by<N(vote_key), const_mem_fun<proposal, uint64_t, &proposal::by_vote_key>>>
      proposal_index;

  vote_index votes;
  ppslvoter_index ppslvoters;
  proposal_index proposals;
};

EOSIO_ABI(ballot, (addvote)(startvote)(addproposal)(voteproposal)(delvoteppsl)(winproposal))