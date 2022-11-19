import * as React from 'react';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import Divider from '@mui/material/Divider';
import ListItemText from '@mui/material/ListItemText';
import ListItemAvatar from '@mui/material/ListItemAvatar';
import Avatar from '@mui/material/Avatar';
import IconButton from '@mui/material/IconButton';
import Typography from '@mui/material/Typography';
import InfoIcon from '@mui/icons-material/Info';

interface IListing {
	name: string;
	abbr: string;
	price: number;
}

interface ListingsProps {
	listings: IListing[];

}

function Listings(props: ListingsProps) {
	const { listings } = props;

	return (
		<List sx={{ width: '100%', maxWidth: 360, bgcolor: 'background.paper' }}>
			{
				listings.map(lst =>
					<ListItem
						key={lst.abbr}
						alignItems="flex-start"
						secondaryAction={
							<IconButton
								edge="end"
								aria-label="comments"
								onClick={() => {
									alert('clicked');
								}}
							>
							  <InfoIcon />
							</IconButton>
						  }
					>

						{/* <ListItemAvatar>
							<Avatar alt="Remy Sharp" src="/static/images/avatar/1.jpg" />
						</ListItemAvatar> */}
						<ListItemText
							primary={lst.name}
							secondary={
								<React.Fragment>
									<Typography
										sx={{ display: 'inline' }}
										component="span"
										variant="body2"
										color="text.primary"
									>
										{lst.abbr}
									</Typography>
									{" — I'll be in your neighborhood doing errands this…"}
								</React.Fragment>
							}
						/>
					</ListItem>
				)
			}
			<Divider variant="inset" component="li" />
		</List>
	);
}