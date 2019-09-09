import { createStore, applyMiddleware } from "redux"
import thunk from "redux-thunk"
import logger from "redux-logger"
import reduxWebsocket from "@giantmachines/redux-websocket";
import rootReducer from "./reducers"
import { WEBSOCKET_PREFIX } from './constants'

const websocketMiddleware = reduxWebsocket({
  prefix: WEBSOCKET_PREFIX,
})

const middleware = [thunk, logger, websocketMiddleware];

const store = createStore(
  rootReducer,
  applyMiddleware(...middleware)
)

export default store
