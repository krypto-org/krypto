import React, { Component } from "react";
import styles from "./Home.scss";
import Layout from "@/layouts/App";
import Instruments from "@/components/Home/Instruments";

class Home extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <Layout>
        <div className={styles.home}>
          <Instruments />
        </div>
      </Layout>
    );
  }
}

export default Home;
