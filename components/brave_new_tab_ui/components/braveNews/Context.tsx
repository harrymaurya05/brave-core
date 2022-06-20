import { useMemo, useState } from "react";
import * as React from "react";

interface BraveNewsContext {
    page: string | null;
    setPage: (page: string | null) => void;
}

export const BraveNewsContext = React.createContext<BraveNewsContext>({
    page: null,
    setPage: () => {}
});

export function BraveNewsContextProvider(props: { children: React.ReactNode }) {
    const [page, setPage] = useState<string | null>(null);
    const context = useMemo(() => ({
        page,
        setPage
    }), [page]);
    return <BraveNewsContext.Provider value={context}>
        {props.children}
    </BraveNewsContext.Provider>
}

export const useBraveNews = () => {
    return React.useContext(BraveNewsContext);
}
