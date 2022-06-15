import Button from "$web-components/button";
import * as React from "react";
import styled, { css } from "styled-components";
import Flex from "../Flex";
import { Heart, HeartOutline } from "./Icons";

export const Container = styled(Flex)`
`;

export const Card = styled('div') <{ backgroundColor?: string, backgroundImage?: string }>`
    position: relative;
    height: 80px;
    background-color: ${p => p.backgroundColor};
    border-radius: 16px;
    ${p => p.backgroundImage && `background-image: url('${p.backgroundImage}');`}
`

export const FollowButton = styled(Button)`
    position: absolute;
    right: 8px;
    top: 8px;

    ${p => !p.isPrimary && css`
        color: var(--interactive5);
        --inner-border-color: var(--interactive5);
        --outer-border-color: var(--interactive5);
    `}
`

export const Name = styled.span`
    font-size: 14px;
    font-weight: 600;
`

export default function FeedCard(props: {
    backgroundColor?: string;
    backgroundImage?: string;
    following: boolean;
    name: string;
}) {
    return <Container direction="column" gap={8}>
        <Card backgroundColor={props.backgroundColor} backgroundImage={props.backgroundImage}>
            <FollowButton isPrimary={!props.following} onClick={console.log}>
                {props.following
                    ? <>
                        {Heart} Following
                    </>
                    : <>
                        {HeartOutline} Follow
                    </>}
            </FollowButton>
        </Card>
        <Name>
            {props.name}
        </Name>
    </Container>
}
