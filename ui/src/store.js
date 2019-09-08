import { createStore, applyMiddleware } from "redux"
import thunk from "redux-thunk"
import logger from "redux-logger"
import reduxWebsocket from "@giantmachines/redux-websocket";
import rootReducer from "./reducers"

const reduxWebsocketMiddleware = reduxWebsocket();

const middleware = [thunk, logger, reduxWebsocketMiddleware];

const store = createStore(
  rootReducer,
  applyMiddleware(...middleware)
)

export default store
