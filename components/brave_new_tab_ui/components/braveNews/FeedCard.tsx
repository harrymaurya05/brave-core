import { useState } from "react";
import * as React from "react";
import styled, { css, keyframes } from "styled-components";
import { isPublisherEnabled, Publisher, setPublisherEnabled } from "../../api/brave_news";
import Flex from "../Flex";
import FollowButton from "./FollowButton";
import { Heart, HeartOutline } from "./Icons";
import { useCachedValue } from "../../hooks/useCachedValue";

const Container = styled(Flex)`
`;

const Card = styled('div') <{ backgroundColor?: string, backgroundImage?: string }>`
    position: relative;
    height: 80px;
    background-color: ${p => p.backgroundColor};
    border-radius: 16px;
    overflow: hidden;
    box-shadow: 0px 0px 16px 0px #63696E2E;
    ${p => p.backgroundImage && `background-image: url('${p.backgroundImage}');`}
`

const StyledFollowButton = styled(FollowButton)`
    position: absolute;
    right: 8px;
    top: 8px;
`

const Name = styled.span`
    font-size: 14px;
    font-weight: 600;
`

const Pulse = keyframes`
    0% { opacity: 0; }
    5% { opacity: 1; }
    80% { opacity: 1; }
    100% { opacity: 0; }
`

const HeartOverlay = styled(Flex)`
    transition: visibility 0.1s 2s;
    
    pointer-events: none;
    background: white;
    color: #aeb1c2;
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    opacity: 0;
    animation: ${Pulse} 2s ease-in-out;
`

const HeartContainer = styled.div`
    width: 32px;
    height: 32px;

    > svg {
        width: 100%;
        height: 100%;
    }
`

export default function FeedCard(props: {
    publisher: Publisher;
    backgroundColor?: string;
    backgroundImage?: string;
    following: boolean;
    name: string;
}) {
    const [following, setFollowing] = useCachedValue(isPublisherEnabled(props.publisher), newValue => setPublisherEnabled(props.publisher, newValue));
    const [toggled, setToggled] = useState(false);
    const toggle = () => {
        setToggled(true);
        setFollowing(!following);
    }
    return <Container direction="column" gap={8}>
        <Card backgroundColor={props.backgroundColor} backgroundImage={props.backgroundImage}>
            <StyledFollowButton following={following} onClick={toggle} />

            {toggled && <HeartOverlay key={following + ''} align="center" justify="center">
                <HeartContainer>
                    {following ? Heart : HeartOutline}
                </HeartContainer>
            </HeartOverlay>}
        </Card>
        <Name>
            {props.name}
        </Name>
    </Container>
}
