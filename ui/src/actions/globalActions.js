export const GLOBAL_SIDEBAR_ON_COLLAPSE = "GLOBAL_SIDEBAR_ON_COLLAPSE"

export const globalSidebarOnCollapse = (collapsed) => ({
  type: GLOBAL_SIDEBAR_ON_COLLAPSE,
  payload: collapsed
})

export const onCollapse = (collapsed) => {
  return (dispatch) => {
    dispatch(globalSidebarOnCollapse(collapsed))
  }
}
