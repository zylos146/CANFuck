import type { PageLoad } from './$types';

export const load = (async () => {
	return { title: 'System Logs' };
}) satisfies PageLoad;
