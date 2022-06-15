import Button from "$web-components/button";
import Toggle from "$web-components/toggle";
import * as React from "react";
import styled from "styled-components";
import Flex from "../Flex";
import FeedCard from "./FeedCard";
import { BackArrow, Heart } from "./Icons";

const Container = styled.div`
    height: 100%;
    overflow: auto;
`

const BackButton = styled(Button)`
    justify-self: start;
`

const Header = styled(Flex)`
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-template-rows: auto;
    justify-self: center;
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

const colors = [
    'black',
    'green',
    'white',
    'salmon'
]

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
            <BackButton onClick={console.log}>
                {BackArrow} Back
            </BackButton>
            <HeaderText>
                {categoryName}
            </HeaderText>
        </Header>
        <Flex direction="row" justify="space-between" align="center">
            <div><Toggle/> Select All</div>
            <Button isPrimary onClick={console.log}>
                {Heart}
                Follow
            </Button>
        </Flex>
        <FeedCardsContainer>
            {feeds.map((f, i) => <FeedCard key={f.title} backgroundColor={colors[i%colors.length]} name={f.title} following={i%2==0} />)}
        </FeedCardsContainer>
    </Container>
}
