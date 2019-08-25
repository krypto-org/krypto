import React from "react";
import { Layout } from 'antd';
const { Content, Footer } = Layout;
import { Route } from "react-router-dom";
import routes from "../routes"

class ContentPage extends React.Component {
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

export default ContentPage;
