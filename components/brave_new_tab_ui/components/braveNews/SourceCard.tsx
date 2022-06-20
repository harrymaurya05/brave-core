import styled from 'styled-components';
import * as React from 'react';
import Flex from "../Flex";
import { useBraveNews } from './Context';

const Container = styled(Flex) <{ background?: string }>`
    font-weight: 600;
    font-size: 14px;
    border-radius: 8px;
    background: ${p => p.background};
    padding: 16px 20px;
    color: white;

    :hover {
        opacity: 0.8;
    }
`

export default function SourceCard(props: { icon: React.ReactNode, text: string, id: string, background?: string }) {
    const { setPage } = useBraveNews();

    return <Container
        onClick={() => setPage(`category/${props.id}`)}
        direction='column'
        justify='end'
        align='start'
        background={props.background}
    >
        {props.icon}
        {props.text}
    </Container>
}
