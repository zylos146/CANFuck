<script lang="ts">
  import CodeMirror from "svelte-codemirror-editor";
  import { javascript } from "@codemirror/lang-javascript";
	import { onMount, onDestroy } from "svelte";
	import { user } from '$lib/stores/user';
	import { page } from '$app/stores';

  let value = "";

  let socket
	const ws_token = $page.data.features.security ? '?access_token=' + $user.bearer_token : '';
  onMount(() => {
    socket = new WebSocket('ws://' + $page.url.host + '/ws/script' + ws_token);
    socket.onopen = (event) => {

    };

    socket.onmessage = (event) => {

	  };
  })
	
	onDestroy(() => socket !== undefined ? socket.close() : null);

  const onCodeChange = (event) => {

  }

  const onCodeSave = () {
    socket.send(JSON.stringify({
      type: 'code',
      filename: 'test.js',
      body: value
    }));
  }

  const onCodeRun = () {
    
  }
  
</script>

<CodeMirror bind:value on:change={onCodeChange} lang={javascript()} editable={true} placeholder={'Testing'}/>