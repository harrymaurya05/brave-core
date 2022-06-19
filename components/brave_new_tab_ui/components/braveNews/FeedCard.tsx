import * as React from "react";
import styled from "styled-components";
import { Publisher, togglePublisher } from "../../api/brave_news";
import Flex from "../Flex";
import FollowButton from "./FollowButton";

export const Container = styled(Flex)`
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

export default function FeedCard(props: {
    publisher: Publisher;
    backgroundColor?: string;
    backgroundImage?: string;
    following: boolean;
    name: string;
}) {
    return <Container direction="column" gap={8}>
        <Card backgroundColor={props.backgroundColor} backgroundImage={props.backgroundImage}>
            <StyledFollowButton following={props.following} onClick={() => togglePublisher(props.publisher)}/>
        </Card>
        <Name>
            {props.name}
        </Name>
    </Container>
}
