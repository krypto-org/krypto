import React, { Component } from "react";
import { Breadcrumb } from "antd";
import styles from "./Path.scss";

class Path extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <Breadcrumb className={styles.path}>
        <Breadcrumb.Item>Home</Breadcrumb.Item>
        <Breadcrumb.Item>List</Breadcrumb.Item>
        <Breadcrumb.Item>App</Breadcrumb.Item>
      </Breadcrumb>
    );
  }
}

export default Path;
