import * as React from 'react';
import List from '@mui/material/List';
import Divider from '@mui/material/Divider';
import ListItemText from '@mui/material/ListItemText';
import ListItemAvatar from '@mui/material/ListItemAvatar';
import Avatar from '@mui/material/Avatar';
import IconButton from '@mui/material/IconButton';
import Typography from '@mui/material/Typography';
import InfoIcon from '@mui/icons-material/Info';

import { useNavigate  } from "react-router-dom";
import ListItemButton from '@mui/material/ListItemButton';
import { useStore } from '../state/store';


const dummy_data : IListing[] = [
  {
    name: "Tesla",
    abbr: "TSX",
    price: 30,
  },
  {
    name: "Apple",
    abbr: "APX",
    price: 230,
  },
  {
    name: "TUM",
    abbr: "TUM",
    price: 95234,
  }
]

interface IListing {
	name: string;
	abbr: string;
	price: number;

}

interface ListingsProps {

}

export function ListingsComponent(props: ListingsProps) {
	// const { listings } = props;
  const store = useStore();
  console.log(store)
  const listings = dummy_data;
  const navigate = useNavigate();



	return (
		<List sx={{ width: '100%', bgcolor: 'background.paper' }}>
			{
				store.staticListings.map(lst =>
          <React.Fragment>
					<ListItemButton
						key={lst.ticker}
						alignItems="flex-start"
            onClick={() => {
              navigate(`/listing/${lst.ticker}`);
            }}
					>

						{/* <ListItemAvatar>
							<Avatar alt="Remy Sharp" src="/static/images/avatar/1.jpg" />
						</ListItemAvatar> */}
              <ListItemText
                primary={
                  <Typography
                    sx={{ display: 'inline' }}
                    component="span"
                    variant="h6"
                    color="text.primary"
                  >
                    {lst.name}
                  </Typography>}
                secondary={ 
                    <Typography
                      sx={{ display: 'inline' }}
                      component="span"
                      variant="subtitle1"
                      color="text.secondary"
                    >
                      { " — " + lst.ticker}
                      {store.listingsWithPrice[lst.ticker]?.price}
                    </Typography>
                }
              />
					</ListItemButton>
          <Divider component="li" />
          </React.Fragment>
				)
			}
		</List>
	);
}