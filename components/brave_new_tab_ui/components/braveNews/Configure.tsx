import { Toggle } from "../../components/toggle";
import * as React from "react";
import styled from "styled-components";
import Flex from "../Flex";
import SourceEntry from "./SourceEntry";
import Discover from "./Discover";
import { BackArrow, Cross } from "./Icons";
import Button from "$web-components/button";

const Grid = styled.div`
  width: 100%;
  height: 100%;

  display: grid;
  grid-template-columns: 250px auto;
  grid-template-rows: 64px 2px auto;

  grid-template-areas:
    "back-button header"
    "separator separator"
    "sidebar content";
`;

const Header = styled.div`
  grid-area: header;
  align-items: center;
  display: flex;
  gap: 12px;
  padding: 12px;
`;

const HeaderText = styled.span`
  font-size: 16px;
`;

const CloseButton = styled(Button)`
  --inner-border-size: 0;
  --outer-border-size: 0;
  padding: 12px;
  width: 32px;
  height: 32px;
`

const BackButtonContainer = styled.div`
  grid-area: back-button;
  align-items: center;
  display: flex;
  padding: 12px;
`;

const BackButton = styled.button`
  all: unset;
  cursor: pointer;
  display: flex;
  gap: 12px;
  align-items: center;

  :hover {
    opacity: 0.8;
  }
`;

const BackButtonText = styled.span`
  font-size: 12px;
  line-height: 1;
`;

const Hr = styled.hr`
  grid-area: separator;
  width: 100%;
  align-self: center;
`;

const Sidebar = styled.div`
  grid-area: sidebar;
  padding: 12px;
`;

const SidebarTitle = styled.span`
  font-size: 18px;
  font-weight: 800;
  line-height: 36px;
`;

const SidebarSubtitle = styled.span`
  font-weight: 500;
  font-size: 12px;
  color: #868e96;
`;

const Content = styled.div`
  grid-area: content;
  padding: 12px;
`;

export default function Configure() {
  const fakeModel = {
    sources: [
      { title: "First" },
      { title: "Second" },
      { title: "Third" },
      { title: "Fourth" }
    ]
  };
  return (
    <Grid>
      <BackButtonContainer>
        <BackButton>
          {BackArrow}
          <BackButtonText>
            Back to <b>Dashboard</b>
          </BackButtonText>
        </BackButton>
      </BackButtonContainer>
      <Header>
        <HeaderText>Brave News</HeaderText>
        <Toggle></Toggle>
        <CloseButton onClick={console.log}>{Cross}</CloseButton>
      </Header>
      <Hr />
      <Sidebar>
        <Flex direction="row" justify="space-between" align="center">
          <SidebarTitle>Following</SidebarTitle>
          <SidebarSubtitle>{fakeModel.sources.length} sources</SidebarSubtitle>
        </Flex>
        <Flex direction="column">
          {fakeModel.sources.map((s, i) => (
            <SourceEntry key={i} sourceId={s.title} />
          ))}
        </Flex>
      </Sidebar>
      <Content>
        <Discover />
      </Content>
    </Grid>
  );
}
