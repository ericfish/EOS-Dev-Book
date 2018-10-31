import React from 'react';

import Post from './Post/Post';

const Posts = ({ posts, deletePost, editPost, likePost }) => {
  return posts.map((post, index) => {
    return (
      <Post
        post={post}
        deletePost={deletePost}
        editPost={editPost}
        likePost={likePost}
        key={index}
      />
    );
  });
};

export default Posts;
