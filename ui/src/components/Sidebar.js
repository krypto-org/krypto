import React from "react";

import { Layout, Menu, Icon } from 'antd';
const { Sider } = Layout;
import { Link } from "react-router-dom";
import routes from "../routes"
import PropTypes from "prop-types"
import { onCollapse } from "../actions/globalActions"
import { connect } from "react-redux";


class Sidebar extends React.Component {
  render() {
    return (
      <Sider collapsible collapsed={this.props.collapsed} onCollapse={this.props.onCollapse}>
        <div className="logo" />
        <Menu theme="dark" defaultSelectedKeys={['0']} mode="inline">
          {routes.map((route, index) => (
            <Menu.Item key={index.toString()}>
              <Icon type={route.icon} />
              <span>{route.sidebar}</span>
              <Link to={route.path} />
            </Menu.Item>
          ))}
        </Menu>
      </Sider>
    )
  }
}

Sidebar.propTypes = {
  collapsed: PropTypes.bool.isRequired,
  onCollapse: PropTypes.func.isRequired
}

const mapStateToProps = (state, ownProps) => ({
  ...ownProps,
  collapsed: state.global.collapsed
})

const mapDispatchToProps = (dispatch) => {
  return {
    onCollapse: (collapsed) => dispatch(onCollapse(collapsed))
  }
}

export default connect(mapStateToProps, mapDispatchToProps)(Sidebar);
