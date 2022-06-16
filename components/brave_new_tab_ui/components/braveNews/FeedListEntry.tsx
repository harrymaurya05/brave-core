import { useCallback } from 'react';
import * as React from 'react';
import styled from 'styled-components';
import Flex from "../Flex";
import { Heart, HeartOutline } from './Icons';
import getBraveNewsController, { isPublisherEnabled, Publisher, UserEnabled } from '../../api/brave_news';

interface Props {
    publisher: Publisher;
}

const Container = styled(Flex)`
    padding: 10px 0;
    cursor: pointer;
    
    :hover {
        opacity: 0.5;
    }
`

const FavIcon = styled.img`
    width: 24px;
    height: 24px;
    border-radius: 100px;
`
const ToggleButton = styled.button`
    all: unset;
    cursor: pointer;
    color: var(--interactive5);
`

const Text = styled.span`
    font-size: 14px;
    font-weight: 500;
`

window['braveNews'] = getBraveNewsController();

export default function FeedListEntry(props: Props) {
    const { publisher } = props;
    const toggleEnabled = useCallback(() => {
        const controller = getBraveNewsController();
        controller.setPublisherPref(publisher.publisherId, isPublisherEnabled(publisher) ? UserEnabled.DISABLED : UserEnabled.ENABLED);
    }, [publisher]);

    return <Container direction="row" justify="space-between" align='center' onClick={toggleEnabled}>
        <Flex align='center' gap={8}>
            <FavIcon src={''} />
            <Text>{publisher.publisherName}</Text>
        </Flex>
        <ToggleButton>
            {isPublisherEnabled(publisher) ? Heart : HeartOutline}
        </ToggleButton>
    </Container>
}
