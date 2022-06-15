import { useCallback, useEffect, useState } from "react";
import { addChangeListener, getPreferences, sendSavePref } from "../api/preferences";

type PrefName = keyof NewTab.Preferences;
type PrefType<T extends PrefName> = NewTab.Preferences[T];
type PrefListener<Pref extends PrefName = PrefName> = (name: Pref, newValue: PrefType<Pref>, oldValue: PrefType<Pref>) => void;

const prefListeners: Map<string & keyof NewTab.Preferences, PrefListener[]> = new Map();
let lastPrefs: NewTab.Preferences;
getPreferences().then(p => lastPrefs = p);
addChangeListener(prefs => {
  for (const [key, listeners] of prefListeners.entries()) {
    const oldValue = lastPrefs[key];
    const newValue = prefs[key];
    if (oldValue === newValue) continue;

    for (const listener of listeners) {
      listener(key, newValue, oldValue);
    }
  }
  lastPrefs = prefs;
});

export function addPrefListener<T extends PrefName>(pref: T, listener: PrefListener<T>) {
  if (!prefListeners.has(pref))
    prefListeners.set(pref, []);

  prefListeners.get(pref)!.push(listener);
}

export function removePrefListener<T extends PrefName>(pref: T, listener: PrefListener<T>) {
  const entry = prefListeners.get(pref);
  if (!entry) return false;

  const index = entry.indexOf(listener);
  entry.splice(index, 1);
  return index !== -1;
}

// Similar API to useState, but for getting/settings NewTab preferences.
export function usePref<T extends PrefName>(name: T) {
  const setPref = useCallback((newValue: NewTab.Preferences[T]) => sendSavePref(name, newValue), [name]);
  const [value, setValue] = useState(lastPrefs[name]);

  useEffect(() => {
    const listener: PrefListener<T> = (_, newValue) => setValue(newValue);
    addPrefListener(name, listener);
    return () => {
      removePrefListener(name, listener);
    }
  }, [name])

  return [
    value,
    setPref
  ] as const;
}
