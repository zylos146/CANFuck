<script lang="ts">
	import type { PageData } from './$types';
	import Serial from './Serial.svelte';

	export let data: PageData;
</script>

<div class="mx-8 my-8 flex flex-col gap-4">
	<Serial />
</div>
