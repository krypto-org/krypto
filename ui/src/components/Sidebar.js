import React from "react";

import { Layout, Menu, Icon } from 'antd';
const { Sider } = Layout;
import { Link } from "react-router-dom";
import routes from "../routes"


class Sidebar extends React.Component {
  render() {
    return (
      <Sider collapsible collapsed={true} onCollapse={this.onCollapse}>
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

export default Sidebar;
