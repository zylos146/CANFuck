<script lang="ts">
	import type { PageData } from './$types';
	import IDE from './IDE.svelte';

	export let data: PageData;
</script>

<div
	class="mx-8 my-8 flex
     flex-col space-y-4"
>
	<IDE />
</div>
