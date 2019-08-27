import React from "react";
import { Layout } from 'antd';
const { Content, Footer } = Layout;
import { Route } from "react-router-dom";
import routes from "../routes"
import PropTypes from "prop-types"
import { connect } from 'react-redux';
import { fetchInstruments } from "../actions/instrumentActions"

class ContentPage extends React.Component {
  componentDidMount() {
    this.props.fetchInstruments();
  }

  render() {
    return (
      <Layout>
        <Content style={{ margin: '10px', padding: "10px" }}>
          {routes.map((route, index) => (
            <Route exact={route.exact} key={index} path={route.path} component={route.comp} />
          ))}
        </Content>
        <Footer style={{ textAlign: 'center' }}>Krypto Trading ©2019</Footer>
      </Layout>
    )
  }
}

ContentPage.propTypes = {
  fetchInstruments: PropTypes.func.isRequired
}

const mapDispatchToProps = (dispatch) => {
  return {
    fetchInstruments: () => dispatch(fetchInstruments())
  }
}

export default connect(null, mapDispatchToProps)(ContentPage);
