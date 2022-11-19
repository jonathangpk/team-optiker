import { AppBar, BottomNavigation, BottomNavigationAction, Box, IconButton, Toolbar, Typography } from "@mui/material";
import { Link, useNavigate } from "react-router-dom";
import ArrowBackIcon from '@mui/icons-material/ArrowBack';
import ShowChartIcon from '@mui/icons-material/ShowChart';
import FormatListBulletedIcon from '@mui/icons-material/FormatListBulleted';
import FeedIcon from '@mui/icons-material/Feed';

interface ContainerProps {
  children: React.ReactNode;
  title: string;
  navigationPosition: string,
}

export function Container({ children, title, navigationPosition }: ContainerProps) {
  const navigate = useNavigate();
  return (
    <Box>
      <AppBar component="nav">
        <Toolbar>
          {
            (navigationPosition !== "home") ? (
              <IconButton
                size="large"
                edge="start"
                color="inherit"
                aria-label="open drawer"
                sx={{ mr: 2 }}
                onClick={() => navigate(-1)}
              >
                <ArrowBackIcon fontSize="large"/>
              </IconButton>
            ) : null
          }
          <Typography variant={navigationPosition === "home" ? "h3" : "h4"}>{title}</Typography>
        </Toolbar>
      </AppBar>
      <Box
        style={{
          marginTop: 80,
          width: '100%',
          position: "fixed",
          // display: "flex",
          justifyContent: "center"
        }}>
          {children}
      </Box>
      <BottomNavigation
          style={{
            width: '100%',
            position: 'fixed',
            bottom: 0
          }}
          showLabels={true}
          value={navigationPosition}
        >
          <BottomNavigationAction icon={<ShowChartIcon fontSize="large"/>} label="Home" value="home" component={Link} to="/" />
          <BottomNavigationAction icon={<FormatListBulletedIcon fontSize="large"/>} label="Listings" value="listings" component={Link} to="/listings" />
          <BottomNavigationAction icon={<FeedIcon fontSize="large"/>} label="News" value="news" component={Link} to="/news" />
        </BottomNavigation>
    </Box>
  );
}