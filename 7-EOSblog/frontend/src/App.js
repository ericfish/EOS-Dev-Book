import React, { Component } from 'react';

import EOSClient from './lib/eos-client';
import CreatePost from './CreatePost/CreatePost';
import Posts from './Posts/Posts';
import './App.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      loading: false,
      posts: []
    };
    const contractAccount = process.env.REACT_APP_EOS_ENV === 'local' ? process.env.REACT_APP_EOS_LOCAL_CONTRACT_ACCOUNT : process.env.REACT_APP_EOS_TEST_CONTRACT_ACCOUNT;
    this.eos = new EOSClient(contractAccount, contractAccount);
    this.loadPosts();
  }

  loadPosts = () => {
    this.eos
      .getTableRows('post')
      .then(data => {
        console.log(data);
        this.setState({ posts: data.rows });
      })
      .catch(e => {
        console.error(e);
      });
  };

  createPost = post => {
    this.setState({ loading: true });

    this.setState({ posts: [...this.state.posts, post] });

    this.eos
      .transaction(
        process.env.REACT_APP_EOS_ACCOUNT,
        'createpost', {
        author: process.env.REACT_APP_EOS_ACCOUNT,
        ...post
      })
      .then(res => {
        console.log(res);
        this.setState({ loading: false });
      })
      .catch(err => {
        this.setState({ loading: false });
        console.log(err);
      });
  };

  deletePost = (pkey, e) => {
    this.setState(prevState => ({
      posts: prevState.posts.filter((post, index) => post.pkey !== pkey)
    }));

    this.eos
      .transaction(process.env.REACT_APP_EOS_ACCOUNT,
        'deletepost', 
        {
          pkey
        })
      .then(res => {
        console.log(res);
        this.setState({ loading: false });
      })
      .catch(err => {
        this.setState({ loading: false });
        console.log(err);
      });
  };

  editPost = (post, e) => {
    this.eos
      .transaction(process.env.REACT_APP_EOS_ACCOUNT,
        'editpost', 
        {
          ...post
        })
      .then(res => {
        console.log(res);
        this.setState({ loading: false });
      })
      .catch(err => {
        this.setState({ loading: false });
        console.log(err);
      });
  };

  likePost = (pkey, e) => {
    this.eos
      .transaction(
        process.env.REACT_APP_EOS_ACCOUNT, 
        'likepost', {
          pkey
        })
      .then(res => {
        console.log(res);
        this.setState({ loading: false });
      })
      .catch(err => {
        this.setState({ loading: false });
        console.log(err);
      });
  };

  render() {
    return (
      <div className="App">
        <div className="pure-g">
          <div className="pure-u-1">
            <Posts
              posts={this.state.posts}
              deletePost={this.deletePost}
              editPost={this.editPost}
              likePost={this.likePost}
            />
            <CreatePost createPost={this.createPost} />
          </div>
        </div>
      </div>
    );
  }
}

export default App;
