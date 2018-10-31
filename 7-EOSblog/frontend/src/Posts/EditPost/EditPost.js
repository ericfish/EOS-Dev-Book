import React, { Component } from 'react';
import { Card, Form, Icon, Input } from 'antd';
const FormItem = Form.Item;
const { TextArea } = Input;

class EditPost extends Component {
  constructor(props) {
    super(props);
    this.state = {
      post: {
        title: this.props.post.title,
        content: this.props.post.content,
        tag: this.props.post.tag
      }
    };
  }

  handleOnChange = e => {
    let post = Object.assign({}, this.state.post);
    post[e.target.name] = e.target.value;
    this.setState({ post });
  };

  render() {
    return (
      <div className="pure-g EditPost">
        <div className="pure-u-4-24" />
        <div className="pure-u-16-24">
          <Form className="EditPost">
            <Card
              actions={[
                <Icon
                  type="save"
                  onClick={e => {
                    this.props.savePost(this.state.post, e);
                  }}
                  style={{ color: '#13c2c2' }}
                />
              ]}
            >
              <FormItem>
                <Input
                  name="title"
                  onChange={this.handleOnChange}
                  value={this.state.post.title}
                  prefix={<Icon type="edit" style={{ color: 'rgba(0,0,0,.25)' }} />}
                  placeholder="Title"
                />
              </FormItem>
              <FormItem>
                <TextArea
                  name="content"
                  onChange={this.handleOnChange}
                  value={this.state.post.content}
                  rows={4}
                  placeholder="Content"
                />
              </FormItem>
              <FormItem>
                <Input
                  name="tag"
                  onChange={this.handleOnChange}
                  value={this.state.post.tag}
                  prefix={<Icon type="tag" style={{ color: 'rgba(0,0,0,.25)' }} />}
                  placeholder="Tag"
                />
              </FormItem>
            </Card>
          </Form>
        </div>
        <div className="pure-u-4-24" />
      </div>
    );
  }
}

export default EditPost;
