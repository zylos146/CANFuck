<script lang="ts">
	import type { PageData } from './$types';
	import Terminal from './Terminal.svelte';

	export let data: PageData;
</script>

<div
	class="mx-8 my-8 flex
     flex-col space-y-4"
>
	<Terminal />
</div>
