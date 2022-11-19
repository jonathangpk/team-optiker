import { AppBar, BottomNavigation, BottomNavigationAction, Typography } from "@mui/material";
import { Link } from "react-router-dom";


interface ContainerProps {
  children: React.ReactNode;
  title: string;
}
export function Container({ children, title }: ContainerProps) {
  return (
    <div>
      <AppBar>
        <Typography variant="h6">{title}</Typography>
      </AppBar>
      {children}
      <BottomNavigation
      showLabels={true}
      onChange={(event, newValue) => {
        
      }}
      >
        <BottomNavigationAction label="Home" component={Link} to="/" />
        <BottomNavigationAction label="Listings" component={Link} to="/listings" />
        <BottomNavigationAction label="News" component={Link} to="/news" />
      </BottomNavigation>
    </div>
  );


}