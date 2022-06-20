import * as React from "react";
import styled, { keyframes } from "styled-components";
import { isPublisherEnabled, Publisher, togglePublisher } from "../../api/brave_news";
import Flex from "../Flex";
import FollowButton from "./FollowButton";
import { Heart, HeartOutline } from "./Icons";

export const Container = styled(Flex)`
    position: relative;
`;

export const Card = styled('div') <{ backgroundColor?: string, backgroundImage?: string }>`
    position: relative;
    height: 80px;
    background-color: ${p => p.backgroundColor};
    border-radius: 16px;
    ${p => p.backgroundImage && `background-image: url('${p.backgroundImage}');`}
`

export const StyledFollowButton = styled(FollowButton)`
    position: absolute;
    right: 8px;
    top: 8px;
`

export const Name = styled.span`
    font-size: 14px;
    font-weight: 600;
`

const Pulse = keyframes`
    0% { opacity: 0; }
    50% { opacity: 1; }
    100% { opacity: 0; }
`

export const HeartOverlay = styled('div')<{ following: boolean }>`
    pointer-events: none;
    background: white;
    color: #aeb1c2;
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;

    animation: ${p => p.following ? Pulse : Pulse} 2s ease-in-out;
`

export default function FeedCard(props: {
    publisher: Publisher;
    backgroundColor?: string;
    backgroundImage?: string;
    following: boolean;
    name: string;
}) {
    const following = isPublisherEnabled(props.publisher);
    return <Container direction="column" gap={8}>
        <Card backgroundColor={props.backgroundColor} backgroundImage={props.backgroundImage}>
            <StyledFollowButton following={props.following} onClick={() => togglePublisher(props.publisher)} />
        </Card>
        <Name>
            {props.name}
        </Name>
        <HeartOverlay following={following}>
            {following ? Heart : HeartOutline}
        </HeartOverlay>
    </Container>
}
