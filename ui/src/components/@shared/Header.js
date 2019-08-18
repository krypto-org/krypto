import React, { Component } from "react";
import { Layout, Menu } from "antd";
import styles from "./Header.scss";

class Header extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <Layout.Header className={styles.header}>
        <div className={styles.logo} />
        <Menu
          theme="dark"
          mode="horizontal"
          defaultSelectedKeys={["1"]}
          className={styles.menu}
        >
          <Menu.Item key="1">Instruments</Menu.Item>
          <Menu.Item key="2">Market Data</Menu.Item>
          <Menu.Item key="3">Orders</Menu.Item>
          <Menu.Item key="4">Algos</Menu.Item>
        </Menu>
      </Layout.Header>
    );
  }
}

export default Header;
