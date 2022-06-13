import { Toggle } from '../../components/toggle';
import * as React from 'react';
import styled from 'styled-components';

const Grid = styled.div`
    width: 100%;
    height: 100%;

    display: grid;
    grid-template-columns: 250px auto;
    grid-template-rows: 64px 2px auto;

    grid-template-areas:
        "back-button header"
        "separator separator"
        "sidebar content"
`;

const Header = styled.div`
    grid-area: header;
    background: red;
    align-items: center;
    display: flex;
    gap: 12px;
    padding: 12px;
`

const HeaderText = styled.span`
    font-size: 16px;
`;

const BackButtonContainer = styled.div`
    grid-area: back-button;
    background: green;
    align-items: center;
    display: flex;
    padding: 12px;
`

const BackButton = styled.button`
    all: unset;
    cursor: pointer;
    display: flex;
    gap: 4px;
    align-items: center;

    :hover {
        opacity: 0.8;
    }
`

const BackButtonText = styled.span`
    font-size: 12px;
`

const LargeText = styled.span`
    font-size: x-large;
`

const Hr = styled.hr`
    grid-area: separator;
    width: 100%;
    align-self: center;
`

const Sidebar = styled.div`
    grid-area: sidebar;
    background: blue;
`

const Content = styled.div`
    grid-area: content;
    background: yellow;
`

export default function Configure() {
    return <Grid>
        <BackButtonContainer>
            <BackButton>
                <LargeText>&lt;</LargeText>
                <BackButtonText>Back to <b>Dashboard</b></BackButtonText>
            </BackButton>
        </BackButtonContainer>
        <Header>
            <HeaderText>Brave News</HeaderText>
            <Toggle></Toggle>
        </Header>
        <Hr />
        <Sidebar>

        </Sidebar>
        <Content>

        </Content>
    </Grid>
}
