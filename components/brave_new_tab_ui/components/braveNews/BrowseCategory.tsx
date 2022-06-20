import Button from "$web-components/button";
import Toggle from "$web-components/toggle";
import * as React from "react";
import styled from "styled-components";
import { isPublisherEnabled } from "../../api/brave_news";
import usePublishers from "../../hooks/braveNews";
import Flex from "../Flex";
import { useBraveNews } from "./Context";
import FeedCard from "./FeedCard";
import FollowButton from "./FollowButton";
import { BackArrow } from "./Icons";

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
    gap: 40px 16px;
`

const colors = [
    'gray',
    'green',
    'salmon'
]

export default function BrowseCategory(props: { categoryId: string }) {
    const { setPage } = useBraveNews()
    const publishers = usePublishers()
    // TODO: When we have categoryIds, use those instead.
    publishers.filter(p => p.categoryName == props.categoryId);

    return <Container>
        <Header direction="row" align="center">
            <BackButton onClick={() => setPage('news')}>
                {BackArrow} Back
            </BackButton>
            <HeaderText>
                {props.categoryId}
            </HeaderText>
        </Header>
        <Flex direction="row" justify="space-between" align="center">
            <div><Toggle /> Select All</div>
            <FollowButton following={false} onClick={console.log} />
        </Flex>
        <FeedCardsContainer>
            {publishers.map((p) => <FeedCard
                key={p.publisherId}
                publisher={p}
                backgroundColor={colors[p.publisherName.length % colors.length]}
                name={p.publisherName}
                following={isPublisherEnabled(p)} />)}
        </FeedCardsContainer>
    </Container>
}
