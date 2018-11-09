import React, { Component } from 'react';
import { Card, Icon } from 'antd';
import { Tag } from 'antd';

import EditPost from '../EditPost/EditPost';
import './Post.css';

class Post extends Component {
  constructor(props) {
    super(props);
    this.state = {
      editing: false,
      liked: false,
      post: {
        title: this.props.post.title,
        content: this.props.post.content,
        tag: this.props.post.tag,
        likes: this.props.post.likes
      }
    };
  }

  savePost = (post, e) => {
    const updatePost = Object.assign(this.props.post, post);
    this.props.editPost(updatePost, e);
    this.setState(prevState => ({
      editing: !prevState.editing,
      post
    }));
  };

  render() {
    const editing = this.state.editing;
    return (
      <div>
        {!editing ? (
          <div className="pure-g Post">
            <div className="pure-u-4-24" />
            <div className="pure-u-16-24">
              <Card
                title={this.state.post.title}
                actions={[
                  <Icon
                    type="delete"
                    onClick={e => {
                      this.props.deletePost(this.props.post.pkey, e);
                    }}
                    style={{ color: '#f5222d' }}
                  />,
                  <Icon
                    onClick={e => {
                      this.setState(prevState => ({
                        editing: !prevState.editing
                      }));
                    }}
                    type="edit"
                    style={{ color: '#13c2c2' }}
                  />,
                  <Icon
                    onClick={e => {
                      this.setState(prevState => ({
                        liked: !prevState.liked,
                        post: Object.assign(prevState.post, {
                          likes: prevState.liked ? prevState.post.likes : prevState.post.likes + 1
                        })
                      }));
                      this.props.likePost(this.props.post.pkey, e);
                    }}
                    type={this.state.liked ? 'heart' : 'heart-o'}
                    style={{ color: '#eb2f96' }}
                  />
                ]}
              >
                <p>By: {this.props.post.author}</p>
                <p>{this.state.post.content}</p>
                <p>Likes: {this.state.post.likes}</p>
                <Tag color="geekblue">{'#' + this.state.post.tag}</Tag>
              </Card>
            </div>
            <div className="pure-u-4-24" />
          </div>
        ) : (
          <EditPost savePost={this.savePost} post={this.props.post} />
        )}
      </div>
    );
  }
}

export default Post;
