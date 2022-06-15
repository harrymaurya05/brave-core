
import * as React from "react";
import styled from "styled-components";
import Flex from "../Flex";
import FeedListEntry from "./FeedListEntry";

const Title = styled.span`
  font-size: 18px;
  font-weight: 800;
  line-height: 36px;
`;

const Subtitle = styled.span`
  font-weight: 500;
  font-size: 12px;
  color: #868e96;
`;

export default function FeedList() {
    const fakeModel = {
      sources: [
        { title: "First" },
        { title: "Second" },
        { title: "Third" },
        { title: "Fourth" }
      ]
    };

    return <div>
        <Flex direction="row" justify="space-between" align="center">
            <Title>Following</Title>
            <Subtitle>{fakeModel.sources.length} sources</Subtitle>
        </Flex>
        <Flex direction="column">
            {fakeModel.sources.map((s, i) => (
                <FeedListEntry key={i} sourceId={s.title} />
            ))}
        </Flex>
    </div>
}
