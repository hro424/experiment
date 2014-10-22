package com.panasonic.sttd;

import com.panasonic.sttd.Client;
import com.panasonic.sttd.Grammar;

class TestClient {
    public static void main(String[] args) throws Exception {
        Grammar[] grammars = new Grammar[3];
        grammars[0] = new Grammar("dict0", 10);
        grammars[1] = new Grammar("dict1", 11);
        grammars[2] = new Grammar("dict2", 20);

        Client client = new Client();
        client.connect();

        //client.addEventListener(listener1);
        client.start(true, true, 3, "ja-JP", "http://pcpf.panasonic.com",
                     grammars);
        client.stop();
        client.close();
    }
}
