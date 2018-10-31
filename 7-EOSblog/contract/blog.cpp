#include "blog.hpp"

using namespace eosio;
using std::string;

class blog : public contract
{
  // blog class inherits the “contract” smart contract and use its constructor below
  using contract::contract;

public:
  // contract constructor
  explicit blog(account_name self) : contract(self) {}

  // mark with @abi action so that eosiocpp will add this as an action to the ABI

  //@abi action
  void createpost(const account_name author, const string &title, const string &content, const string &tag)
  {
    // check if authorized for account to create a blog post
    // if you are not authorized then this action will be aborted
    // and the transaction will by rolled back - any modifications will be reset
    require_auth(author);

    // post_index is our multi_index
    // multi_index is how you store persistant data across actions in EOSIO
    // each action has a new action context which is a clean working memory with no prior working state from other action executions
    post_index posts(_self, _self); // code, scope

    // add a record to our multi_index posts
    // const_iterator emplace( unit64_t payer, Lambda&& constructor )
    // TODO change payer
    posts.emplace(author, [&](auto &post) {
      post.pkey = posts.available_primary_key();
      post.author = author;
      post.title = title;
      post.content = content;
      post.tag = tag;
      post.likes = 0;
    });
  }

  //@abi action
  void deletepost(const uint64_t pkey)
  {
    post_index posts(_self, _self);

    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey could not be found");

    // check if authorized to delete post
    require_auth(iterator->author);

    posts.erase(iterator);
  }

  //@abi action
  void editpost(const uint64_t pkey, const string &title, const string &content, const string &tag)
  {
    post_index posts(_self, _self);

    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey could not be found");

    // check if authorized to update post
    require_auth(iterator->author);

    posts.modify(iterator, iterator->author, [&](auto &post) {
      post.title = title;
      post.content = content;
      post.tag = tag;
    });
  }

  //@abi action
  void likepost(const uint64_t pkey)
  {
    // do not require_auth since want to allow anyone to call

    post_index posts(_self, _self);

    // verify already exist
    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey not found");

    posts.modify(iterator, 0, [&](auto &post) {
      print("Liking: ",
            post.title.c_str(), " By: ", post.author, "\n");
      post.likes++;
    });
  }

private:
  // mark with @abi table so that eosiocpp will add this as a multi_index to the ABI with an index of type i64

  //@abi table post i64
  struct post
  {
    uint64_t pkey;
    uint64_t author;
    string title;
    string content;
    string tag;
    uint64_t likes = 0;

    auto primary_key() const { return pkey; }

    uint64_t get_author() const { return author; }
    string get_tag() const { return tag; }

    // call macro
    EOSLIB_SERIALIZE(post, (pkey)(author)(title)(content)(tag)(likes))
  };

  // typedef multi_index<N(table_name), object_template_to_use, other_indices> multi_index_name;
  typedef multi_index<N(post), post,
                      indexed_by<N(byauthor), const_mem_fun<post, uint64_t, &post::get_author>>>
      post_index;

  // TODO add another index for tag
  // typedef multi_index<N(post), post,
  //                     indexed_by<N(byauthor), const_mem_fun<post, uint64_t, &post::get_author>>,
  //                     indexed_by<N(bytag), const_mem_fun<post, string, &post::get_tag>>>
  //     post_index;
};

EOSIO_ABI(blog, (createpost)(deletepost)(likepost)(editpost))