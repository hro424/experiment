package com.panasonic.sttd;

public class Grammar {
    private String name;
    private int weight;

    public Grammar(String name, int weight) {
        this.name = name;
        this.weight = weight;
    }

    public String name() {
        return this.name;
    }

    public int weight() {
        return this.weight;
    }
}
