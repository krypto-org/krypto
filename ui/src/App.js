import React from "react"
import { BrowserRouter } from "react-router-dom"
import Layout from "./components/ApplicationLayout"
import { Provider } from "react-redux"
import store from "store"
import { connect } from '@giantmachines/redux-websocket'

store.dispatch(connect('ws://192.168.1.12:5757'));

class App extends React.Component {
  render() {
    return (
      <Provider store={store}>
        <BrowserRouter>
          <Layout />
        </BrowserRouter>
      </Provider>
    );
  }
}

export default App
