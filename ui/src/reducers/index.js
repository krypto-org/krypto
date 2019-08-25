import { combineReducers } from 'redux';
import instrumentReducer from './instrumentsReducer';

export default combineReducers({
  instruments: instrumentReducer
});
