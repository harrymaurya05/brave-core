import Button from "$web-components/button";
import * as React from "react";
import styled from "styled-components";
import Flex from "../Flex";
import { StyledCheckbox } from "../toggle/style";
import CategoryCard from "./CategoryCard";
import { BackArrow, Heart } from "./Icons";

const Container = styled.div`
    height: 100%;
    overflow: auto;
`

const Header = styled(Flex)`
    > * {
        flex: 1;
    }
`

const HeaderText = styled(Flex)`
    font-weight: 500;
    font-size: 16px;
`

const FeedCardsContainer = styled('div')`
    display: grid;
    grid-template-columns: repeat(3, auto);
    grid-template-columns: repeat(auto-fill, auto);
`

export default function BrowseCategory(props: { categoryId: string }) {
    const feeds = [
        { title: 'First' },
        { title: 'Second' },
        { title: 'Third' },
        { title: 'Fourth' },
    ]
    const categoryName = "Business";
    return <Container>
        <Header direction="row" align="center">
            <Button onClick={console.log}>
                {BackArrow} Back
            </Button>
            <HeaderText>
                {categoryName}
            </HeaderText>
            {/* Placeholder div, so everything is centered nicely */}
            <div />
        </Header>
        <Flex direction="row" justify="space-between" align="center">
            <span><StyledCheckbox />Select All</span>
            <Button isPrimary onClick={console.log}>
                {Heart}
                Follow
            </Button>
        </Flex>
        <FeedCardsContainer>
            {feeds.map(f => <CategoryCard key={f.title} backgroundUrl="" categoryId={f.title} text={f.title} />)}
        </FeedCardsContainer>
    </Container>
}
