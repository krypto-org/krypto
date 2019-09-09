import React from "react"
import { BrowserRouter } from "react-router-dom"
import Layout from "./components/ApplicationLayout"
import { Provider } from "react-redux"
import store from "store"

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
