import { GLOBAL_SIDEBAR_ON_COLLAPSE } from "../actions/globalActions"

const initialState = {
  collapsed: true
};

export default function globalReducer(state = initialState, action) {
  switch (action.type) {
    case GLOBAL_SIDEBAR_ON_COLLAPSE:
      return {
        collapsed: action.payload
      };
    default:
      return state;
  }
}
