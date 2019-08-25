import React from "react";
import ReactDOM from "react-dom";

import { Provider } from "react-redux";
import { createStore } from "redux";
// import thunk from "redux-thunk";
// import logger from "redux-logger";
import rootReducer from "./reducers";
import App from "App";

import "@/themes/App.global.scss";

const store = createStore(
  rootReducer
)

ReactDOM.render(
  <Provider store={store}><App /></Provider>,
  document.getElementById("app")
);
