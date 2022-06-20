import * as React from 'react';
import styled from 'styled-components';
import Configure from './Configure';
import { useBraveNews } from "./Context";

const Dialog = styled.dialog`
    border-radius: 8px;
    border: none;
    width: min(100vw, 1092px);
    height: min(100vh, 712px);
    z-index: 1000;
    background: white;
    overflow: hidden;
    padding: 0;
`

export default function BraveNewsModal() {
    const { page } = useBraveNews();
    return <Dialog open={!!page}>
        <Configure />
    </Dialog>
}
