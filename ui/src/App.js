import React from "react";
import { BrowserRouter } from "react-router-dom";
import Layout from "./components/ApplicationLayout";
import { Provider } from "react-redux";
import { createStore } from "redux";
// import thunk from "redux-thunk";
// import logger from "redux-logger";
import rootReducer from "./reducers";

const store = createStore(
  rootReducer
)

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

export default App;
