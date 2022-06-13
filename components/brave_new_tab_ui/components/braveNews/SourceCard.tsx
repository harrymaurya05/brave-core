import styled from 'styled-components';
import * as React from 'react';
import Flex from "../Flex";

const Container = styled(Flex)<{ background?: string }>`
    font-weight: 600;
    font-size: 14px;
    border-radius: 8px;
    background: ${p => p.background};
    padding: 16px 20px;
    color: white;
`

export default function SourceCard(props: { icon: React.ReactNode, text: string, id: string, background?: string }) {
    return <Container direction='column' justify='end' align='start' background={props.background}>
        {props.icon}
        {props.text}
    </Container>
}
