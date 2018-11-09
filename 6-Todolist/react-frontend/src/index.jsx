import React from 'react'
import ReactDOM from 'react-dom'
import EOS from 'eosjs'
import update from 'react-addons-update';

const EOS_CONFIG = {
  contractName: "todo", // Contract name
  contractSender: "todo", // User executing the contract (should be paired with private key)
  clientConfig: {
    keyProvider: [''], // Your private key
    httpEndpoint: 'http://127.0.0.1:8888' // EOS http endpoint
  }
}

class TodoForm extends React.Component {
  constructor(props) {
    super(props)
    this.state = { descripion: "" }
  }

  updateInput(e){
    this.setState({ description: e.target.value })
  }

  saveTodo(e) {
    e.preventDefault();
    this.props.onSubmit(this.state.description)
    this.setState({ description: "" })
  }

  render() {
    return(
      <form onSubmit={this.saveTodo.bind(this)}>
        <input type="text" value={this.state.description} placeholder="Add a new TODO" onChange={this.updateInput.bind(this) }/>
        <button type="submit">Save</button>
      </form>
    )
  }
}

class TodoList extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      loading: false,
      todos: []
    }
    this.eosClient = EOS.Localnet(EOS_CONFIG.clientConfig)
    this.loadTodos();
  }

  loadTodos() {
    this.eosClient.getTableRows('todos', 'todo', 'todo').then((data) => {
      this.setState({ todos: data })
    }).catch((e) => {
      console.error(e);
    })
  }


  addNewTodo(description){
    this.setState({ loading: true })

    const newTodos = update(this.state.todos, {$push: [
      { id: (this.state.todos.length + 1), description: description, completed: false },
    ]});

    this.setState({ todos: newTodos })

    this.eosClient.contract(EOS_CONFIG.contractName).then((contract) => {
      contract.create(
        EOS_CONFIG.contractSender,
        (this.state.todos.length + 1),
        description,
        { authorization: [EOS_CONFIG.contractSender] }
      ).then((res) => { this.setState({ loading: false }) })
      .catch((err) => { this.setState({ loading: false }); console.log(err) })
    })
  }

  completeTodo(id, e) {
    e.preventDefault();
    this.setState({ loading: true })

    var todoIndex = this.state.todos.findIndex((todo) => { return todo.id == id });

    this.setState({
      todos: update(this.state.todos, {
        [todoIndex]: { $merge: { completed: true }}
      })
    })

    this.eosClient.contract(EOS_CONFIG.contractName).then((contract) => {
      contract.complete(
        EOS_CONFIG.contractSender,
        (this.state.todos.length + 1),
        { authorization: [EOS_CONFIG.contractSender] }
      ).then((res) => { this.setState({ loading: false }) })
      .catch((err) => { this.setState({ loading: false }); console.log(err) })
    })
  }

  removeTodo(id, e) {
    e.preventDefault();
    this.setState({ loading: true })

    var todoIndex = this.state.todos.findIndex((todo) => { return todo.id == id });
    this.setState({ todos: this.state.todos.filter(todo => todo.id != id) })

    this.eosClient.contract(EOS_CONFIG.contractName).then((contract) => {
      contract.destroy(
        EOS_CONFIG.contractSender,
        (this.state.todos.length + 1),
        { authorization: [EOS_CONFIG.contractSender] }
      ).then((res) => { this.setState({ loading: false }) })
      .catch((err) => { this.setState({ loading: false }); console.log(err) })
    })
  }

  renderTodoItem(todo) {
    return (
      <li key={todo.id}>
        {todo.completed ?
         <span>[x] </span> :
         <input type="checkbox" onClick={this.completeTodo.bind(this, todo.id)} checked={false} /> }
        {todo.description}
        { " " }
        {todo.completed ? <a href="#" onClick={this.removeTodo.bind(this, todo.id)}>(remove)</a> : ""}
      </li>
    );
  }

  render() {
    return (
      <div>
        <h3>My TODOs: {this.state.loading ? <small>(saving...)</small> : ""}</h3>
        {this.state.todos.map(this.renderTodoItem.bind(this))}
        <br />
        <TodoForm onSubmit={this.addNewTodo.bind(this)} />
      </div>
    );
  }
}

ReactDOM.render(<TodoList />, document.getElementById('app'));

module.hot.accept();
