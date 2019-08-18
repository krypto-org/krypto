import React, { Component } from "react";
import PropTypes from "prop-types";
import { Layout } from "antd";

import styles from "./App.scss";

import Header from "@/components/@shared/Header";
import Footer from "@/components/@shared/Footer";

class App extends Component {
  static propTypes = {
    children: PropTypes.node.isRequired
  };

  constructor(props) {
    super(props);
  }

  render() {
    const { children } = this.props;
    return (
      <Layout>
        <Header />
        <Layout className={styles.app}>
          <Layout.Content>{children}</Layout.Content>
        </Layout>
        <Footer />
      </Layout>
    );
  }
}

export default App;
